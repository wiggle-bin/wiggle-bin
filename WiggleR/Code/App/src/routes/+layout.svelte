<script lang="ts">
	import { goto } from '$app/navigation';
	import { onMount } from 'svelte';
	import ImageSelector from '../lib/ImageSelector.svelte';

	let openImageSelector: Boolean | 'start' | 'end' = false
	let startImage = ''
	let endImage = ''

	function close() {
		openImageSelector = false
	}

  function scheduleStart() {
		openImageSelector = 'start'
	}

  function scheduleEnd() {
		openImageSelector = 'end'
	}

	function imageSelected (e: CustomEvent) {
    if (openImageSelector === 'start') {
      startImage = e.detail
    }
    if (openImageSelector === 'end') {
      endImage = e.detail
    }
    openImageSelector = false
	}

	onMount(async () => {
		await import('@kor-ui/kor');
		await import('@kor-ui/kor/kor-styles.css');
	});
</script>

<kor-page>
	<kor-app-bar slot="top" label="WiggleR">
		<kor-tabs>
			<kor-tab-item on:click={() => goto('/timelapse')} label="Timelapse" />
			<kor-tab-item on:click={() => goto('/timeline')} label="Timeline" />
			<kor-tab-item on:click={() => goto('/test')} label="Test" />
		</kor-tabs>

    <div class="actionsBar">
      <kor-button icon="schedule" label={startImage || 'Start'} on:click={scheduleStart} /> 
      <kor-button icon="schedule" label={endImage || 'End'} on:click={scheduleEnd} /> 
    </div>
    {#if openImageSelector}
      <ImageSelector on:select={imageSelected} on:close={close} />
    {/if}

	</kor-app-bar>

	<slot />
</kor-page>

<style>
  .actionsBar {
    display: flex;
    gap: 10px;
  }
</style>