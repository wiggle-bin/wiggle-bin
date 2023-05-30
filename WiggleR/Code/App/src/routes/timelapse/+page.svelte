<script>
	import { start, end } from '../../store/store';

	let startTime = '';
	let endTime = '';

	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));
</script>

{#key [startTime, endTime]}
	<video controls>
		<source
			src={`http://localhost:8000/video?startTime=${startTime}&endTime=${endTime}`}
			type="video/mp4"
		/>
	</video>
{/key}
