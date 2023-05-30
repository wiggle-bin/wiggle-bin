<script lang="ts">
	import { start, end } from '../store/store';
	import { goto } from '$app/navigation';
	import { onMount } from 'svelte';
	import ImageSelector from '../lib/ImageSelector.svelte';

	let openImageSelector: Boolean | 'start' | 'end' = false;
	let startTime = '';
	let endTime = '';

	function close() {
		openImageSelector = false;
	}

	function scheduleStart() {
		openImageSelector = 'start';
	}

	function scheduleEnd() {
		openImageSelector = 'end';
	}

	function imageSelected(e: CustomEvent) {
		if (openImageSelector === 'start') {
			start.update((start) => (start = e.detail));
		}
		if (openImageSelector === 'end') {
			end.update((end) => (end = e.detail));
		}
		openImageSelector = false;
	}

	onMount(async () => {
		await import('@kor-ui/kor');
		await import('@kor-ui/kor/kor-styles.css');
	});

	start.subscribe((start) => (startTime = start));
	end.subscribe((end) => (endTime = end));
</script>

<kor-page>
	<kor-app-bar slot="top" label="WiggleR">
		<kor-tabs>
			<kor-tab-item on:click={() => goto('/timelapse')} label="Timelapse" />
			<kor-tab-item on:click={() => goto('/timeline')} label="Timeline" />
			<kor-tab-item on:click={() => goto('/test')} label="Test" />
		</kor-tabs>

		<div class="actionsBar">
			<kor-button icon="schedule" label={startTime || 'Start'} on:click={scheduleStart} />
			<kor-button icon="schedule" label={endTime || 'End'} on:click={scheduleEnd} />
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
